namespace ShopApp.Models
{
    /// <summary>
    /// Information about Product
    /// </summary>
    public class Product
    {
        /// <summary>
        /// Unique Id in system
        /// </summary>
        public Guid Id { get; set; }

        /// <summary>
        /// Product description from the manufacturer
        /// </summary>
        public string Description { get; set; }

        /// <summary>
        /// Recommended price from the manufacturer
        /// </summary>
        public double Price { get; set; }
    }
}